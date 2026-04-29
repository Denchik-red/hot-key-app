import "./config"

import express from "express";
import { prisma } from "./prisma"

const app = express();
const PORT = parseInt(process.env.PORT || "3000");

app.use(express.json())

app.get("/", async (req, res) => {
  res.send("Hello World!");
  const users = await prisma.user.create({
    data: {
      name: "den",
      DeathCounter: {
        create: {
          count: 0,
        }
      }
    },
    include: {
      DeathCounter: true
    }
  });
  console.log(users)
});

app.get("/users", async (req, res) => {
  const users = await prisma.user.findMany({
    include: {
      DeathCounter: true
    }
  });
  res.send(users);
})

app.post("/users", async (req, res) => {
  if (!req.body.name) {
    res.status(400).send("Name is required");
    return;
  }
  const name = req.body.name;
  const count = req.body.count ? req.body.count : 0;

  const user = {
    name: name,
    DeathCounter: {
      create: {
        count: count,
      }
    }
  }
  try {
    const users = await prisma.user.create(
      {
        data: user,
        include: {
          DeathCounter: true,
        }
      }
    )
    res.send(users);

  } catch (e) {
    console.log(e)
    res.status(400).send("User already exists");
    return;
  }
})

app.listen(PORT, () => {
  console.log(`Server start in http://localhost:${PORT}`);
});

export default app;